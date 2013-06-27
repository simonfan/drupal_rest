<?php
/**
 * Define the itemQuery class
 */
class REST_EntityFieldQuery extends EntityFieldQuery {

	// a public property storesthe EntityFieldQuery (efq) query result
	public $efq_result;

	// save options
	private $options = array();

	// items that are the result
	public $items = array();		

	/**
	 * apply some defaults to all instances of this object
	 */
	/*
		$options = array(
			'entity_type' => 'string',
			'id_field_value' => 'string',
			'required' => array(),				// object to be passed on to the rest.item
			'start' => 0,
			'length' => 30
		);
	*/
	public function __construct($options) {
		// save options
		$this->options = $options;

		// paging
		$start = isset($options['start']) ? $options['start'] : 0;
		$length = isset($options['length']) ? $options['length'] : 0;

		$this
			// entity type
			->entityCondition('entity_type', $options['entity_type'])
			// Default to published
			->propertyCondition('status', 1)
			// define a default range for the query
			// starting at 0 and ending in 20
			->range( $start , $length );
	}

	/**
	 * Override the default 'execute()' method.
	 */
	public function execute() {
		// get the entity type
		$entity_type = $this->options['entity_type'];

		// get the requirement object
		$requirements = $this->options['required'];

		// call parent "execute()" method.
		$this->efq_result = parent::execute();

		// with the response of the parent execute query, 
		// extract required fields.
		if ( !empty( $this->efq_result[ $entity_type ] ) ) {
			// if there are any results,

			$ids = array_keys($this->efq_result[ $entity_type ]);

			// load all necessary entities
			$entities = entity_load( $entity_type, $ids );

			foreach ($entities AS $entity) {

				$item = new REST_EntityItem($entity_type, $entity, $requirements);
				$item = $item->export();

				// add the item to the items array
				array_push($this->items, $item);
			}

		} else {
			// no results, return a message
		}

		return $this;
	}

	public function export() {
		return $this->items;
	}

}